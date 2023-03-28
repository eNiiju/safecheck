CREATE TABLE `Course` (
  `courseId` int PRIMARY KEY AUTO_INCREMENT,
  `nomCourse` varchar(255)
);

CREATE TABLE `Personne` (
  `personneId` int PRIMARY KEY AUTO_INCREMENT,
  `nom` varchar(255),
  `prenom` varchar(255)
);

CREATE TABLE `Balise` (
  `baliseId` int PRIMARY KEY AUTO_INCREMENT
);

CREATE TABLE `Participation` (
  `dossard` int PRIMARY KEY,
  `personneId` int,
  `courseId` int
);

CREATE TABLE `Passage` (
  `horodatage` timestamp,
  `dossard` int,
  `baliseId` int,
  PRIMARY KEY (`dossard`, `baliseId`)
);

CREATE TABLE `Checkpoint` (
  `baliseId` int,
  `courseId` int,
  `longitude` float8,
  `latitude` float8,
  PRIMARY KEY (`baliseId`, `courseId`)
);

ALTER TABLE `Participation` ADD FOREIGN KEY (`personneId`) REFERENCES `Personne` (`personneId`);

ALTER TABLE `Participation` ADD FOREIGN KEY (`courseId`) REFERENCES `Course` (`courseId`);

ALTER TABLE `Checkpoint` ADD FOREIGN KEY (`baliseId`) REFERENCES `Balise` (`baliseId`);

ALTER TABLE `Checkpoint` ADD FOREIGN KEY (`courseId`) REFERENCES `Course` (`courseId`);

ALTER TABLE `Passage` ADD FOREIGN KEY (`dossard`) REFERENCES `Participation` (`dossard`);

ALTER TABLE `Passage` ADD FOREIGN KEY (`baliseId`) REFERENCES `Balise` (`baliseId`);
