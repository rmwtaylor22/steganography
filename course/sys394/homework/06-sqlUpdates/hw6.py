from flask import Flask, render_template, g, flash, redirect, url_for
from flask_wtf import FlaskForm
from wtforms import StringField, SubmitField, PasswordField
from wtforms.validators import Email, Length, InputRequired, EqualTo


import db # if error, right-click parent directory "mark directory as" "sources root"

app = Flask(__name__)
app.config['SECRET_KEY'] = 'Super Secret Unguessable Key'


@app.before_request
def before_request():
    db.open_db_connection()


@app.teardown_request
def teardown_request(exception):
    db.close_db_connection()


@app.route('/')
def index():
    return render_template('index.html')


@app.route('/trips')
def all_accounts():
    results = db.trip_report()
    print(results)

    return render_template('trip-report.html', accounts=results)

class MemberForm(FlaskForm):
    destination = StringField('Destination', validators=[Length(min=1, max=40)])
    semester = SelectField('Semester', choices=[('fall', 'Fall'), ('jterm', 'Interterm'), ('spring', 'Winter'), ('winter', 'Winter'))])
    year = StringField('Year', validators=[Length(min=1, max=40)])
    first_name = StringField('First Name', validators=[Length(min=1, max=40)])
    last_name = StringField('Last Name', validators=[Length(min=1, max=40)])
    student_class = StringField('Class', validators=[Length(min=1, max=40)])
    submit = SubmitField('Save Trip')

app.run(debug=True)