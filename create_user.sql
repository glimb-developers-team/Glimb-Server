--
-- Create the table users
--

CREATE TABLE user (
	login VARCHAR(16) NOT NULL PRIMARY KEY,
	password VARCHAR(16) NOT NULL,
	type VARCHAR(16) NOT NULL
);
