from os import environ
from flask import Flask, render_template, send_file, request
import cymysql

app = Flask(__name__, template_folder=environ["FLASK_DIRECTORY"] + "/src/html")
mysql = cymysql.connect(host='127.0.0.1', user='root', passwd='', db='insecapp')

class Post(object) :
    def __init__(self, id, title, content, url, likes=0):
        self.id = id
        self.title = title
        self.content = content
        self.url = url
        self.likes = likes

@app.route('/')
def mainpage() :
    posts = []
    cursor = mysql.cursor()
    cursor.execute('SELECT url, title, description, likes, id FROM posts')

    for r in cursor.fetchall():
        posts.append(Post(r[4], r[1], r[2], r[0], r[3]))

    cursor.close()

    return render_template('main.html', posts=posts)

# Static Routing
@app.route('/css/<string:style_sheet>')
def staticstyles(style_sheet) :
    return send_file(environ["FLASK_DIRECTORY"] + '/src/css/%s' % style_sheet)

@app.route('/img/<string:image>')
def staticimg(image) :
    return send_file(environ["FLASK_DIRECTORY"] + '/src/img/%s' % image)

@app.route('/js/<string:script>')
def staticscript(script) :
    return send_file(environ["FLASK_DIRECTORY"] + '/src/js/%s' % script)

# favicon
@app.route('/favicon.ico')
def favicon() :
    return send_file(environ["FLASK_DIRECTORY"] + '/src/img/favicon.ico')

# API calls
@app.route('/new-post', methods=["POST"])
def newPost() :
    data = request.form
    url = data["url"]
    title = data["title"]
    description = data["content"]

    cursor = mysql.cursor()
    try :
        cursor.execute('INSERT INTO posts (url, title, description) VALUES ("%s", "%s", "%s")' % (url, title, description))
    except Exception as e :
        return str(e)

    r = cursor.rowcount
    cursor.close()

    if r > 0 :
        return "success"
    
    return "error"

@app.route('/like-post', methods=["POST"])
def likePost() :
    data = request.form
    postid = data["id"]

    cursor = mysql.cursor()

    try :
        cursor.execute('UPDATE posts SET likes=likes + 1 WHERE id=%s' % postid)
    except Exception as e :
        return str(e)

    r = cursor.rowcount
    cursor.close()

    if r > 0 :
        return "success"
    
    return "error"

if __name__ == '__main__' :
    app.run()