SELECT people.name
FROM movies
INNER JOIN stars ON movies.id=stars.movie_id
INNER JOIN people ON stars.person_id=people.id
WHERE title ="Toy Story";