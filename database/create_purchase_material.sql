--
-- Create table purchase_material
--

CREATE TABLE purchase_material (
	PurchaseId INT NOT NULL,
	MaterialTitle VARCHAR(32) NOT NULL,
	Cuantity INT,
	FOREIGN KEY (PurchaseId) REFERENCES purchase (Id),
	FOREIGN KEY (MaterialTitle) REFERENCES material (Title),
	PRIMARY KEY (PurchaseId, MaterialTitle)
);
