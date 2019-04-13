SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION';


CREATE SCHEMA IF NOT EXISTS 'social_network' DEFAULT CHARACTER SET utf8 ;
USE 'social_network' ;

-- -----------------------------------------------------
-- Table 'social_network'.'user'
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS 'social_network'.'user' (
  'user_id' INT NOT NULL AUTO_INCREMENT,
  'password' VARCHAR(50) NOT NULL,
  'name' VARCHAR(50) NOT NULL,
  'birthday' DATE NULL DEFAULT NULL,
  'gender' ENUM("MALE", "FEMALE") NULL DEFAULT NULL,
  'address' TEXT(200) NULL DEFAULT NULL,
  PRIMARY KEY ('user_id'),
  UNIQUE INDEX 'name_UNIQUE' ('name' ASC) )
ENGINE = InnoDB
AUTO_INCREMENT = 1000;


-- -----------------------------------------------------
-- Table 'social_network'.'education'
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS 'social_network'.'education' (
  'education_id' INT NOT NULL AUTO_INCREMENT,
  'user_id' INT NOT NULL,
  'level' VARCHAR(50) NOT NULL,
  'start' DATE NOT NULL,
  'end' DATE NOT NULL,
  'school_name' VARCHAR(50) NOT NULL,
  'degree' VARCHAR(30) NOT NULL,
  PRIMARY KEY ('education_id'),
  INDEX 'user_id_idx' ('user_id' ASC) ,
  CONSTRAINT 'education_user_id'
    FOREIGN KEY ('user_id')
    REFERENCES 'social_network'.'user' ('user_id')
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB
AUTO_INCREMENT = 1000;


-- -----------------------------------------------------
-- Table 'social_network'.'work_experience'
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS 'social_network'.'work_experience' (
  'experience_id' INT NOT NULL AUTO_INCREMENT,
  'user_id' INT NOT NULL,
  'employer' VARCHAR(50) NOT NULL,
  'start' DATE NOT NULL,
  'end' DATE NOT NULL,
  'position' VARCHAR(50) NOT NULL,
  PRIMARY KEY ('experience_id'),
  INDEX 'user_id_idx' ('user_id' ASC) ,
  CONSTRAINT 'work_experience_user_id'
    FOREIGN KEY ('user_id')
    REFERENCES 'social_network'.'user' ('user_id')
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB
AUTO_INCREMENT = 1000;


-- -----------------------------------------------------
-- Table 'social_network'.'email'
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS 'social_network'.'email' (
  'user_id' INT NOT NULL,
  'address' VARCHAR(50) NOT NULL,
  PRIMARY KEY ('address'),
  INDEX 'user_id_idx' ('user_id' ASC) ,
  CONSTRAINT 'email_user_id'
    FOREIGN KEY ('user_id')
    REFERENCES 'social_network'.'user' ('user_id')
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table 'social_network'.'friend_group'
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS 'social_network'.'friend_group' (
  'group_id' INT NOT NULL AUTO_INCREMENT,
  'user_id' INT NOT NULL,
  'group_name' VARCHAR(100) NOT NULL,
  PRIMARY KEY ('group_id'),
  INDEX 'user_id_idx' ('user_id' ASC) ,
  UNIQUE INDEX 'group_name_UNIQUE' ('group_name' ASC) ,
  UNIQUE INDEX 'group_id_UNIQUE' ('group_id' ASC) ,
  CONSTRAINT 'friend_group_user_id'
    FOREIGN KEY ('user_id')
    REFERENCES 'social_network'.'user' ('user_id')
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB
AUTO_INCREMENT = 1000;


-- -----------------------------------------------------
-- Table 'social_network'.'friend'
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS 'social_network'.'friend' (
  'user_id' INT NOT NULL,
  'friend_id' INT NOT NULL,
  'group_id' INT NULL DEFAULT NULL,
  PRIMARY KEY ('user_id', 'friend_id'),
  INDEX 'group_id_idx' ('group_id' ASC) ,
  INDEX 'friendship_friend_id' ('friend_id' ASC) ,
  CONSTRAINT 'friendship_user_id'
    FOREIGN KEY ('user_id')
    REFERENCES 'social_network'.'user' ('user_id')
    ON DELETE CASCADE
    ON UPDATE CASCADE,
  CONSTRAINT 'friendship_friend_id'
    FOREIGN KEY ('friend_id')
    REFERENCES 'social_network'.'user' ('user_id')
    ON DELETE CASCADE
    ON UPDATE CASCADE,
  CONSTRAINT 'friendship_group_id'
    FOREIGN KEY ('group_id')
    REFERENCES 'social_network'.'friend_group' ('group_id')
    ON DELETE SET NULL
    ON UPDATE SET NULL)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table 'social_network'.'log'
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS 'social_network'.'log' (
  'log_id' INT NOT NULL AUTO_INCREMENT,
  'user_id' INT NOT NULL,
  'private' TINYINT NOT NULL DEFAULT 0,
  'content' LONGTEXT NOT NULL,
  'update_time' DATETIME NOT NULL,
  PRIMARY KEY ('log_id'),
  INDEX 'user_id_idx' ('user_id' ASC) ,
  CONSTRAINT 'log_user_id'
    FOREIGN KEY ('user_id')
    REFERENCES 'social_network'.'user' ('user_id')
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB
AUTO_INCREMENT = 1000;


-- -----------------------------------------------------
-- Table 'social_network'.'reply'
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS 'social_network'.'reply' (
  'reply_id' INT NOT NULL AUTO_INCREMENT,
  'user_id' INT NOT NULL,
  'log_id' INT NOT NULL,
  'replied_id' INT NULL DEFAULT NULL,
  'content' MEDIUMTEXT NOT NULL,
  'reply_time' DATETIME NOT NULL,
  PRIMARY KEY ('reply_id'),
  INDEX 'log_id_idx' ('log_id' ASC) ,
  INDEX 'replied_id_idx' ('replied_id' ASC) ,
  INDEX 'reply_user_id' ('user_id' ASC) ,
  CONSTRAINT 'reply_user_id'
    FOREIGN KEY ('user_id')
    REFERENCES 'social_network'.'user' ('user_id')
    ON DELETE CASCADE
    ON UPDATE CASCADE,
  CONSTRAINT 'reply_log_id'
    FOREIGN KEY ('log_id')
    REFERENCES 'social_network'.'log' ('log_id')
    ON DELETE CASCADE
    ON UPDATE CASCADE,
  CONSTRAINT 'reply_replied_id'
    FOREIGN KEY ('replied_id')
    REFERENCES 'social_network'.'reply' ('reply_id')
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB
AUTO_INCREMENT = 1000;


-- -----------------------------------------------------
-- Table 'social_network'.'share'
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS 'social_network'.'share' (
  'share_id' INT NOT NULL AUTO_INCREMENT,
  'user_id' INT NOT NULL,
  'log_id' INT NULL DEFAULT NULL,
  'content' LONGTEXT NOT NULL,
  'share_time' DATETIME NOT NULL,
  PRIMARY KEY ('share_id'),
  INDEX 'user_id_idx' ('user_id' ASC) ,
  INDEX 'share_log_id_idx' ('log_id' ASC) ,
  CONSTRAINT 'share_user_id'
    FOREIGN KEY ('user_id')
    REFERENCES 'social_network'.'user' ('user_id')
    ON DELETE CASCADE
    ON UPDATE CASCADE,
  CONSTRAINT 'share_log_id'
    FOREIGN KEY ('log_id')
    REFERENCES 'social_network'.'log' ('log_id')
    ON DELETE SET NULL
    ON UPDATE SET NULL)
ENGINE = InnoDB
AUTO_INCREMENT = 1000;

USE 'social_network';

SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;

CREATE VIEW friend_information AS
SELECT friend_id,user.name as friend_name, birthday,gender,address
FROM user,friend 
WHERE friend_id=user.user_id;

CREATE VIEW basic_information AS
SELECT user.user_id,name ,birthday ,gender , user.address, email.address as email_address
FROM user,email
WHERE user.user_id = email.user_id;


