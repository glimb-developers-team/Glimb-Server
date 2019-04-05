--
-- Create the table material
--

CREATE TABLE material (
	Title VARCHAR(32) NOT NULL,
	Unit VARCHAR(8) NOT NULL,
	Price DOUBLE UNSIGNED NOT NULL,
	PRIMARY KEY (Title)
);
