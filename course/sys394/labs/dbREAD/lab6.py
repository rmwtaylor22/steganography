import psycopg2
import psycopg2.extras
from flask import Flask, render_template


app = Flask(__name__)


@app.route('/')
def index():
    return render_template('index.html')


@app.route('/accounts')
def all_accounts():

    data_source_name = "dbname=rwhite user=rwhite password=rwhite host=roller.cse.taylor.edu"


    connection = psycopg2.connect(data_source_name)
    cursor = connection.cursor(cursor_factory=psycopg2.extras.DictCursor)

    cursor.execute('SELECT * FROM account')
    results = cursor.fetchall()

    cursor.close()
    connection.close()

    return render_template('all-accounts.html', accounts=results)


app.run(debug=True)