--
-- Create the table purchase
--

CREATE TABLE purchase (
	Id INT AUTO_INCREMENT NOT NULL,
	ClientNum VARCHAR(16) NOT NULL,
	ForemanNum VARCHAR(16) NOT NULL,
	LocalId INT NOT NULL,
	TotalCost DOUBLE UNSIGNED NOT NULL,
	Status BOOLEAN DEFAULT NULL,
	FOREIGN KEY (ClientNum) REFERENCES user (PhoneNumber),
	FOREIGN KEY (ForemanNum) REFERENCES user (PhoneNumber),
	PRIMARY KEY (Id)
);
