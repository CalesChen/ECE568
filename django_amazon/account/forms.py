from django import forms 
from django.contrib.auth.models import User
from django.contrib.auth.forms import UserCreationForm
from .models import UserAccount
from django.forms.fields import DateInput, DateField, NumberInput

class UserRegistrationForm(UserCreationForm):
    email = forms.EmailField()
    first_name = forms.CharField(max_length=100, required=True)
    last_name = forms.CharField(max_length=100,required=True)

    class Meta:
        model = User
        fields = ['username','email','password1','password2','first_name','last_name']