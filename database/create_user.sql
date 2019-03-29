--
-- Create the table users
--

CREATE TABLE user (
	Name VARCHAR(16) NOT NULL,
	LastName VARCHAR(16) NOT NULL,
	MiddleName VARCHAR(16),
	PhoneNumber VARCHAR(16) NOT NULL,
	Password VARCHAR(16) NOT NULL,
	Type VARCHAR(16) NOT NULL,
	ForemanPhoneNumber VARCHAR(16),
	PRIMARY KEY (PhoneNumber)
);
