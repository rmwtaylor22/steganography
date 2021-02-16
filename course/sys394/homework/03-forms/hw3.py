

class MyForm(Form):
    email = StringField(u'First Name', validators=[validators.input_required()])
    username  = StringField(u'Last Name', validators=[validators.optional()])
    password = StringField(u'Last Name', validators=[validators.optional()])