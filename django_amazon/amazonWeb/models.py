from django.db import models
from django.contrib.auth.models import User
from django.utils import timezone
# Create your models here.

class Categories(models.Model):
    category = models.CharField(max_length=20, null = False, blank = False)

    def __str__(self):
        return self.category

class Warehouse(models.Model):
    address_x = models.IntegerField(default=-1, blank=True)
    address_y = models.IntegerField(default=-1, blank=True)

    def __str__(self):
        return f'({self.address_x}, {self.address_y})'
