SELECT title
FROM movies
WHERE id IN
(SELECT movie_id
FROM people
JOIN stars ON people.id=stars.person_id
WHERE name is "Helena Bonham Carter")
AND id IN
(SELECT movie_id
FROM people
JOIN stars ON people.id=stars.person_id
WHERE name is "Johnny Depp");
