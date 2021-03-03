import psycopg2
import psycopg2.extras
from flask import Flask, render_template, g, flash, redirect, url_for

import db # if error, right-click parent directory "mark directory as" "sources root"


app = Flask(__name__)


@app.route('/')
def index():
    return render_template('index.html')


@app.route('/trips')
def trip_report():
    data_source_name = "dbname=rwhite user=rwhite password=rwhite host=roller.cse.taylor.edu"

    db.trip_report
    connection = psycopg2.connect(data_source_name)
    cursor = connection.cursor(cursor_factory=psycopg2.extras.DictCursor)

    cursor.execute('SELECT * FROM trips')
    results = cursor.fetchall()

    cursor.close()
    connection.close()

    return render_template('all-accounts.html', accounts=results)

