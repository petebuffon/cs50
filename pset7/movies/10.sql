SELECT people.name
FROM movies
INNER JOIN directors ON movies.id=directors.movie_id
INNER JOIN ratings ON movies.id=ratings.movie_id
INNER JOIN people ON directors.person_id=people.id
WHERE rating >= 9.0
GROUP BY person_id;