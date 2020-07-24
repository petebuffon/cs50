SELECT name
FROM people
JOIN stars ON people.id=stars.person_id
WHERE movie_id IN
(SELECT movie_id
FROM people
JOIN stars ON people.id=stars.person_id
WHERE name is "Kevin Bacon" AND birth is 1958)
AND NOT name is "Kevin Bacon"
GROUP BY name;