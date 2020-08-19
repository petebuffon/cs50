import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash
import base64
from io import BytesIO
from matplotlib.figure import Figure
import matplotlib.ticker as ticker
from datetime import datetime

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    # Query database for cash
    rows = db.execute("SELECT cash FROM users WHERE id = :id",
                      id=session["user_id"])
    cash = rows[0]["cash"]

    # Query database for stocks
    rows = db.execute("SELECT symbol, symbol_id FROM transactions JOIN symbols ON transactions.symbol_id=symbols.id WHERE user_id = :user_id GROUP BY symbol_id",
                      user_id=session["user_id"])
    stocks = []
    total_cash = cash
    for row in rows:
        query = lookup(row["symbol"])
        shares = db.execute("SELECT SUM(shares) FROM transactions WHERE symbol_id = :symbol_id AND user_id = :user_id",
                            symbol_id=row["symbol_id"], user_id=session["user_id"])[0]["SUM(shares)"]
        total = usd(shares * query["price"])
        total_cash += shares * query["price"]
        stocks.append({"symbol": query["symbol"], "name": query["name"],
                      "shares": shares, "price": usd(query["price"]), "total": total})

    return render_template("index.html", cash=usd(cash), total=usd(total_cash), stocks=stocks)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure symbol was submitted
        if not request.form.get("symbol"):
            return apology("must provide symbol", 403)

        # Ensure shares was submitted
        if not request.form.get("shares"):
            return apology("must provide shares", 403)

        # Ensure symbol valid
        query = lookup(request.form.get("symbol"))
        if not query:
            return apology("invalid symbol", 403)

        # Ensure shares positive integer
        try:
            shares = int(request.form.get("shares"))
        except (KeyError, TypeError, ValueError):
            return apology("invalid shares", 403)
        if shares <= 0:
            return apology("invalid shares", 403)

        # Ensure user has enough cash
        rows = db.execute("SELECT cash FROM users WHERE id = :id",
                          id=session["user_id"])
        cash = rows[0]["cash"]
        cost = shares * query["price"]
        if cash < cost:
            return apology("not enough cash", 403)

        # Query database for symbol
        rows = db.execute("SELECT id FROM symbols WHERE symbol = :symbol",
                          symbol=query["symbol"])

        # Insert symbol in symbols if not already present
        if len(rows) != 1:
            db.execute("INSERT into symbols (symbol) VALUES (:symbol)", symbol=query["symbol"])
            rows = db.execute("SELECT id FROM symbols WHERE symbol = :symbol",
                          symbol=query["symbol"])

        # Insert transaction into database
        db.execute("INSERT into transactions (symbol_id, user_id, shares, price, transacted) VALUES (:symbol_id, :user_id, :shares, :price, datetime('now'))",
                   symbol_id=rows[0]["id"], user_id=session["user_id"], shares=shares, price=query["price"])

        # Update cash
        db.execute("UPDATE users SET cash = :cash WHERE id = :id", cash=cash - cost, id=session["user_id"])

        # Redirect user to home page
        flash("Bought!")
        return redirect("/")

    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    # Query database for transactions
    rows = db.execute("SELECT symbol, shares, price, transacted FROM transactions JOIN symbols ON transactions.symbol_id=symbols.id WHERE user_id = :user_id",
                      user_id=session["user_id"])

    # cash = usd(rows[0]["cash"])
    return render_template("history.html", transactions=rows)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure symbol was submitted
        if not request.form.get("symbol"):
            return apology("must provide symbol", 403)

        # Ensure symbol valid
        quote = lookup(request.form.get("symbol"))
        # Ensure symbol exists
        if not quote:
            return apology("invalid symbol")

        # Redirect user to quoted page
        return render_template("quoted.html", name=quote["name"], price=quote["price"], symbol=quote["symbol"])

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("/quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username doesn't already exist
        if len(rows) == 1:
            return apology("username already exists", 403)
        username = request.form.get("username")

        # Ensure password was submitted
        if not request.form.get("password"):
            return apology("must provide password", 403)
        # Ensure confirmation was submitted
        elif not request.form.get("password"):
            return apology("must confirm password", 403)
        # Ensure password and confirmation match
        elif request.form.get("password") != request.form.get("confirmation"):
            return apology("passwords do not match", 403)
        hash = generate_password_hash(request.form.get("password"))

        # Insert new username and password
        db.execute("INSERT into users (username, hash) VALUES (:username, :hash)", username=username, hash=hash)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=username)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        flash("Registered!")
        return redirect("/")

        # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure symbol was submitted
        if not request.form.get("symbol"):
            return apology("must provide symbol", 403)

        # Ensure shares was submitted
        if not request.form.get("shares"):
            return apology("must provide shares", 403)

        # Ensure symbol valid
        query = lookup(request.form.get("symbol"))
        if not query:
            return apology("invalid symbol", 403)

        # Ensure shares positive integer
        try:
            shares = int(request.form.get("shares"))
        except (KeyError, TypeError, ValueError):
            return apology("invalid shares", 403)
        if shares <= 0:
            return apology("invalid shares", 403)

        # Query database for symbol_id
        symbol_id = db.execute("SELECT id FROM symbols WHERE symbol = :symbol",
                               symbol=query["symbol"])[0]["id"]

        # Ensure has enough shares
        owned_shares = db.execute("SELECT SUM(shares) FROM transactions WHERE symbol_id = :symbol_id AND user_id = :user_id",
                                  symbol_id=symbol_id, user_id=session["user_id"])[0]["SUM(shares)"]
        if owned_shares < shares:
            return apology("not enough shares", 403)

        # Insert transaction into database
        db.execute("INSERT into transactions (symbol_id, user_id, shares, price, transacted) VALUES (:symbol_id, :user_id, :shares, :price, datetime('now'))",
                   symbol_id=symbol_id, user_id=session["user_id"], shares=shares * -1, price=query["price"])

        # Update cash
        cash = db.execute("SELECT cash FROM users WHERE id = :id",
                          id=session["user_id"])[0]["cash"]
        profit = shares * query["price"]
        db.execute("UPDATE users SET cash = :cash WHERE id = :id", cash=cash + profit, id=session["user_id"])

        # Redirect user to home page
        flash("Sold!")
        return redirect("/")

    else:
        return render_template("sell.html")


@app.route("/plot", methods=["GET", "POST"])
@login_required
def plot():
    """Show plot of symbol price over time"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure symbol was submitted
        if not request.form.get("symbol"):
            return apology("must provide symbol", 403)

        # Ensure symbol valid
        query = lookup(request.form.get("symbol"))
        symbol = query["symbol"]
        price = query["price"]
        # Ensure symbol exists
        if not quote:
            return apology("invalid symbol")

        # Select symbol_id
        symbol_id = db.execute("SELECT id FROM symbols WHERE symbol = :symbol",
                               symbol=symbol)[0]["id"]

        # Select transactions
        data = db.execute("SELECT price FROM transactions WHERE symbol_id = :symbol_id",
                          symbol_id=symbol_id)

        # Create price and transacted lists
        prices = []
        for transaction in data:
            prices.append(transaction.get("price"))
        prices.append(price)

        data = db.execute("SELECT transacted FROM transactions WHERE symbol_id = :symbol_id",
                          symbol_id=symbol_id)
        transacted = []
        for transaction in data:
            transacted.append(transaction.get("transacted"))
        transacted.append(datetime.now().isoformat())

        # Generate figure
        fig = Figure()
        ax = fig.subplots()
        ax.plot(transacted, prices)

        # format the coords message box
        formatter = ticker.FormatStrFormatter('$%1.2f')
        ax.yaxis.set_major_formatter(formatter)
        for tick in ax.yaxis.get_major_ticks():
            tick.label1.set_color('green')

        # rotates and right aligns the x labels, and moves the bottom of the
        # axes up to make room for them
        fig.autofmt_xdate()

        # create more room for x-axis
        fig.tight_layout()

        # Save figure as temporary buffer
        buf = BytesIO()
        fig.savefig(buf, format="png")

        # Embed results in html output
        plot = base64.b64encode(buf.getbuffer()).decode("ascii")
        return render_template("plotted.html", title=symbol, plot=f"data:image/png;base64,{plot}")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("/plot.html")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
