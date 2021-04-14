from django.db import models
from django.contrib.auth.models import User
# Create your models here.

class UserAccount(models.Model):
    user = models.OneToOneField(User, primary_key = True, on_delete=models.CASCADE)
    ups_account_name = models.CharField(max_length=50, default="", blank=True)
    address_x = models.IntegerField(default=-1, blank=True)
    address_y = models.IntegerField(default=-1, blank=True)

    def __str__(self):
        return f'{self.user.username} Account Info'
