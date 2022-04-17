from django.contrib import admin
from .models import Profile, Vehicle, Ride, Driver
# Register your models here.

admin.site.register(Profile)
admin.site.register(Vehicle)
admin.site.register(Ride)
admin.site.register(Driver)