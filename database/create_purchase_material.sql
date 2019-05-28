--
-- Create table purchase_material
--

CREATE TABLE purchase_material (
	PurchaseId INT NOT NULL,
	MaterialTitle VARCHAR(32) NOT NULL,
	Quantity INT UNSIGNED,
	Cost DOUBLE UNSIGNED,
	FOREIGN KEY (PurchaseId) REFERENCES purchase (Id),
	FOREIGN KEY (MaterialTitle) REFERENCES material (Title),
	PRIMARY KEY (PurchaseId, MaterialTitle)
);
