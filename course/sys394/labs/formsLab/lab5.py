from flask import Flask, session, redirect, url_for, render_template, flash
from flask_wtf import FlaskForm
from wtforms import StringField, SubmitField, BooleanField, PasswordField
from wtforms.validators import Email, Length, Regexp


app = Flask(__name__)
app.config['SECRET_KEY'] = 'secret key for session application'


class LoginForm(FlaskForm):

    valid_pword = []
    valid_pword.append(Length(min=8))
    valid_pword.append(Regexp(r'.*[A-Za-z]', message="Must have at least one letter"))
    valid_pword.append(Regexp(r'.*[0-9]', message="Must have at least one digit"))
    valid_pword.append(Regexp(r'.*[!@#$%^&*_+=]', message="Must have at least one special character"))

    email = StringField('Email', validators=[Email()])
    password = PasswordField('Password', validators=valid_pword)
    remember = BooleanField('Remember me on this machine')
    submit = SubmitField('Log In')


def authenticate_user(email, pword):

    fake_user_database = {}
    fake_user_database['ds@cse.taylor.edu'] = 'Pa$$word123'
    fake_user_database['bnroller@taylor.edu'] = 'Gr3atteach!'

    if email in fake_user_database:
        if fake_user_database[email] == pword:
            return True

    return False


@app.route('/')
def index():
    return render_template('index.html')


@app.route('/login', methods=['GET', 'POST'])
def login():

    login_form = LoginForm()

    if login_form.validate_on_submit():

        if not authenticate_user(login_form.email.data, login_form.password.data):
            flash('Username and password combination invalid')
        else:
            session['email'] = login_form.email.data
            session['remember'] = login_form.remember.data

            flash('User logged in')
            return redirect(url_for('index'))

    return render_template('login.html', form=login_form)


@app.route('/logout')
def logout():

    session.pop('email', None)
    session.pop('remember', None)

    flash('Logged out')
    return redirect(url_for('index'))


app.run(debug=True)