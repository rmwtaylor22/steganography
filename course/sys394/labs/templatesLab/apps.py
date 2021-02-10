from flask import Flask, render_template

app = Flask(__name__)

@app.route('/')
def hello_world():
    return render_template('base.html')

@app.route('/letters')
def letters():
    return render_template('theLetters.html')

# @app.route('/comments')
# def comments():
#     fake_comments = [ { 'who': 'wesley',
#                         'what': 'As you wish!'},
#                       { 'who': 'vincini',
#                         'what': 'Inconceivable'} ]
#     return render_template('comments.html', comments=fake_comments)

app.run(debug=True)