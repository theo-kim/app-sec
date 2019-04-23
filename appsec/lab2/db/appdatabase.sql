-- This file is run to setup the database for the application
-- Assuming that the database insecapp was created using the file /bin/unix/sql.sh

DROP TABLE IF EXISTS posts;
DROP TABLE IF EXISTS users;

CREATE TABLE IF NOT EXISTS posts (
    id INT NOT NULL AUTO_INCREMENT,
    url VARCHAR(200) NOT NULL,
    title VARCHAR(50) NOT NULL,
    description VARCHAR(400) NOT NULL,
    likes INT NOT NULL DEFAULT 0,
    PRIMARY KEY (id)
);

CREATE TABLE IF NOT EXISTS users (
    id INT NOT NULL AUTO_INCREMENT,
    name VARCHAR(100) NOT NULL,
    password VARCHAR(100) NOT NULL,
    PRIMARY KEY (id, name)
);

-- Populate the database with some samples
INSERT INTO posts (url, title, description, likes) VALUES 
    ("https://blog.syntonic.io/img/slack.png", "Slack is back!", "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nunc est justo, venenatis vel urna ac, pretium bibendum lacus. Aliquam pretium ex nec augue laoreet, vitae hendrerit arcu molestie. Maecenas dignissim, sem nec accumsan laoreet, purus arcu faucibus felis, in pretium felis magna eu ante.", 0),
    ("https://i0.wp.com/dev.slack.com/img/avatars/ava_0010-512.v1443724322.png?ssl=1", "Another Post!", "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nunc est justo, venenatis vel urna ac, pretium bibendum lacus.", 0);

INSERT INTO users (name, password) VALUES
    ('admin', 'admin_password');