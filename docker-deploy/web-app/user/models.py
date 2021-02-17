from django.db import models
from django.contrib.auth.models import User 
from datetime import date

# Create your models here.
# class User(User_Base):
#     #username = models.CharField(max_length=50)
#     Password = models.CharField(max_length=50)
#     emailAddress = models.CharField(max_length=50)
#     is_driver = models.BooleanField(default=False)

class Profile(models.Model):
    user = models.OneToOneField(User, on_delete=models.CASCADE)
    is_driver = models.CharField(default='User', max_length=10)

    def __str__(self):
        return f'{self.user.username} Profile'

class Vehicle(models.Model):
    owner = models.OneToOneField(User, on_delete=models.CASCADE)
    type = models.CharField(default='Car',max_length=50)
    license_number=models.CharField(default='000',max_length=50)
    capacity = models.IntegerField(default=4)
    comment = models.CharField(default='000', max_length=100)
    def __str__(self):
        return f'{self.owner.username} Vehicle'

class Ride(models.Model):
    destination = models.CharField(max_length=100)
    #arrival_date = modles.DateField(default=timezone.now)
    arrival_time = models.DateTimeField()
    passenger_number = models.IntegerField()
    vehicle_type = models.CharField(default='Car', max_length=50, blank=True)
    special_request = models.CharField(max_length= 100, blank=True)
    can_Shared = models.BooleanField(default=False)
    sharer = models.ManyToManyField(User, blank=True, related_name='sharer')
    owner= models.ForeignKey(User, on_delete=models.CASCADE, related_name='owner')
    STATUS = (
        ('open', 'open'),
        ('confirmed', 'confirmed'),
        ('completed', 'completed'),
    )
    status = models.CharField(max_length=20, choices=STATUS, default='open')

    def __str__(self):
        return self.destination

class Driver(models.Model):
    driver = models.ForeignKey(User, on_delete=models.CASCADE)
    ride = models.ForeignKey(Ride, on_delete=models.CASCADE)

    def __str__(self):
        return f'{self.ride.destination} by {self.driver.username}'

