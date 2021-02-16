from django import forms
from user.models import User
from django.contrib.auth.forms import UserCreationForm
from .models import Profile, Vehicle, Ride, Driver
from datetime import datetime, timedelta, tzinfo

class UserRegisterForm(UserCreationForm):
    email = forms.EmailField()
    #is_driver = forms.BooleanField()
    class Meta:
        model = User
        fields = ['username', 'email', 'password1', 'password2']


class UserUpdateForm(forms.ModelForm):
    email = forms.EmailField()
    
    class Meta:
        model = User
        fields = ['username', 'email']


class ProfileUpdateForm(forms.ModelForm):
    is_driver = forms.ChoiceField(initial=2,choices=((1, 'Driver'),(2,'User')))
    class Meta:
        model = Profile 
        fields = ['is_driver']

class VehicleUpdateForm(forms.ModelForm):
    type = forms.CharField()
    license_number = forms.CharField()
    capacity = forms.IntegerField()
    comment = forms.CharField()
    class Meta:
        model = Vehicle 
        fields = ['type', 'license_number', 'capacity', 'comment']

class DateTimeInput(forms.DateTimeInput):
    input_type = 'datetime-local'


class RideRequestForm(forms.ModelForm):
    destination = forms.CharField()
    arrival_time = forms.DateTimeField()
    passenger_number = forms.IntegerField()
    #is_shared = forms.ChoiceField(initial=2,choices=((1, 'True'),(2,'False')))
    special_request = forms.CharField()
    special_vehicle_type = forms.CharField(required=False)    
    class Meta:
        model = Ride
        fields = ['destination', 'arrival_time', 'passenger_number', 'vehicle_type', 'special_request', 'can_Shared']

class RideSearchForm(forms.Form):
    destination = forms.CharField()
    earliest_time = forms.DateTimeField(
        input_formats = ['%Y-%m-%dT%H:%M'],
        widget = DateTimeInput(
            format='%Y-%m-%dT%H:%M',
            attrs={'type': 'datetime-local'})
        )

    latest_time = forms.DateTimeField(
        input_formats = ['%Y-%m-%dT%H:%M'],
        widget = DateTimeInput(
            format='%Y-%m-%dT%H:%M',
            attrs={'type': 'datetime-local'})
        )
    passenger_number = forms.IntegerField()
    #is_shared = forms.ChoiceField(initial=2,choices=((1, 'True'),(2,'False')))
    
class DriverUpdateForm(forms.ModelForm):
    driver = User
    ride = Ride
    class Meta:
        model = Driver 
        fields = ['driver', 'ride']