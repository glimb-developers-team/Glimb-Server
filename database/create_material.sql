--
-- Create the table material
--

CREATE TABLE material (
	Title VARCHAR(128) NOT NULL,
	Units VARCHAR(8) NOT NULL,
	Price DOUBLE UNSIGNED NOT NULL,
	PRIMARY KEY (Title)
);
