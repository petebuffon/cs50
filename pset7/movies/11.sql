SELECT movies.title
FROM movies
INNER JOIN stars ON movies.id=stars.movie_id
INNER JOIN people ON stars.person_id=people.id
INNER JOIN ratings ON movies.id=ratings.movie_id
WHERE name is "Chadwick Boseman"
ORDER BY rating DESC
LIMIT 5;