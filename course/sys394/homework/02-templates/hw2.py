from flask import Flask, render_template

app = Flask(__name__)

@app.route('/')
def hello_world():
    return render_template('hello.html')

@app.route('/name')
def hello_name():
    return render_template('hello-name.html', name='Fred Ziffle')

@app.route('/letters')
def comments():
    fake_comments = [ 'alpha',
                       'beta',
                       'gamma',
                       'delta' ]
    return render_template('letters.html', comments=fake_comments)

if __name__ == '__main__':
    app.run(debug=True)
