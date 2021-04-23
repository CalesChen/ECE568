from django.db import models
from django.contrib.auth.models import User
# Create your models here.

class UserAccount(models.Model):
    user = models.OneToOneField(User, primary_key=True, on_delete=models.CASCADE)
    phone = models.BigIntegerField(default=0)
    cardInfo = models.BigIntegerField(default=0)
    ups_username = models.CharField(max_length=50, default="", blank=True)
    ups_userid = models.IntegerField(default=-1)
    def __str__(self):
        return f'{self.user.username} Account Info'

class Address(models.Model):
    owner = models.ForeignKey(UserAccount, related_name='addresses', on_delete=models.CASCADE)
    addressName = models.CharField(max_length=200, default="")
    tag = models.CharField(max_length=30, default="", blank=True)
    address_x = models.IntegerField(default=-1, blank=True)
    address_y = models.IntegerField(default=-1, blank=True)

    def __str__(self):
        return f'{self.tag}: {self.addressName}, <{self.address_x},{self.address_y}>'