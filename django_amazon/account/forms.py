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

class EditOptionalForm(forms.Form):
    phone = forms.IntegerField()
    cardInfo = forms.IntegerField()
    ups_account_name = forms.CharField(max_length=100,required=True)

class EditProfileForm(forms.Form):
    email = forms.EmailField()
    first_name = forms.CharField(max_length=100, required=True)
    last_name = forms.CharField(max_length=100,required=True)

class AddAddressForm(forms.Form):
    ADDRESS_TAGS = (('home','home'),('home','company'),('home','school'),('home','my own tag'))
    addressName = forms.CharField(max_length=100,required=True)
    tag = forms.CharField(widget=forms.widgets.Select(choices=ADDRESS_TAGS))
    myTag = forms.CharField(max_length=100, required = False)
    address_x = forms.IntegerField(required=True)
    address_y = forms.IntegerField(required=True)