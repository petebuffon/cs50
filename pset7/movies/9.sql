SELECT people.name
FROM movies
INNER JOIN stars ON movies.id=stars.movie_id
INNER JOIN people ON stars.person_id=people.id
WHERE year is 2004
GROUP BY person_id
ORDER BY birth;