# Insecure Web Application
This application allows users to post image links on a public wall with a custom caption.

The application is written using Python (Flask) for the backend, HTML/CSS/JavaScript for the front end, and MySQL for the database. 

## Setting up the application to run
### Linux
1. If running Ubuntu or a similar Debian style Linux distribution that supports `apt` package management, just run `bin/unix/setup.sh` then `bin/unix/sql.sh`. You will be prompted for admin priveledges for the installation.
2. If not running a system with `apt` package management (i.e. macOS), ensure that **Python 3** and `pip`, the package manager for Python, are installed on your machine. Also ensure that `mysql-server` is installed on your system. Then setup the local database using `bin/unix/sql.sh`.
3. Then run the app using `bin/unix/run.sh`.

### Windows
*NOT SUPPORTED...*

### Viewing the App
Open a web browser and navigate to http://localhost:5000

## Vulnerabilities
This application is vulnerable to the following vulnerabilities:

- Cross Site Scripting (XSS)
- SQL Injection

### XSS
The XSS vulnerability arises from an improperly escaped image source attribute which can be made to run a code snippet using the image `onload` tag.

To observe the XSS vulnerability, go to the application and open the "New Post Window". Input the following into the "Image URL" text field:

```
https://blog.syntonic.io/img/slack.png onload = alert(1);
```

This will inject `alert(1)` into the tag such that, when the image loads, the JavaScript code will run. Therefore, on page load, an alert box should show up. 

This vulnerability could allow hackers to post malicious code to the public website to steal potentially secured cookies when the user loads the page.

**POTENTIAL MITIGATION**: This vulnerability is easily mitigated by adding a Content Security Policy (CSP) such that inline JavaScript is not allowed. Alternatively, the Regex check can be made more robust as to check that the entire string represents a URL and a backend, magic byte check can be performed to ensure that the linked file exists and is actually an image.

### SQL Injection
The SQL injection vulnerability arises from an improperly parsed SQL query which does not sanitize the input for quotations and other special characters. Furthermore, the application prints the SQL error messages when they arise directly to the user, allowing the user access to a means of debugging their SQL injection query.

To observe the SQL injection go to the application and open the "New Post Window". You will notice that typing a quote (`"`) will cause an SQL error. However, there are some sanitizing checks performed on the input, such as a regex check that the IMAGE URL string is, indeed, a URL. Using this, you can enter the following inputs to the form:

``` 
Image Title: SQL INJECTION!

IMAGE URL: https://blog.syntonic.io/img/slack.png", "THIS IS THE ADMIN PASSWORD", (SELECT password FROM users WHERE name = "admin")), ("https://blog.syntonic.io/img/slack.png

POST CONTENT: Something something
```
After submitting the above information, you should get the desired output in the post's description (i.e. the admin's password)!

**POTENTIAL MITIGATION**: Just properly escape the strings in the server code and remove obscure MySQL error reporting to the user

**INSECURE CODE:**
```python
data = request.form
    url = data["url"]
    title = data["title"]
    description = data["content"]

    cursor = mysql.cursor()
    try :
        cursor.execute('INSERT INTO posts (url, title, description) VALUES ("%s", "%s", "%s")' % (url, title, description))
    except Exception as e :
        return str(e)
```

**SECURE CODE:** (using libraries built in sanitization functionality)
```python
data = request.form
url = data["url"]
title = data["title"]
description = data["content"]

cursor = mysql.cursor()
try :
    cursor.execute('INSERT INTO posts (url, title, description) VALUES (%s, %s, %s)', url, title, description)
except Exception as e :
    return str(e)
```