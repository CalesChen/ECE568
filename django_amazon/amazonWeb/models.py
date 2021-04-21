from django.db import models
from django.contrib.auth.models import User
from django.utils import timezone
# Create your models here.

class Category(models.Model):
    category = models.CharField(max_length=20, null = False, blank = False)

    def __str__(self):
        return self.category

class Warehouse(models.Model):
    address_x = models.IntegerField(default=-1, blank=True)
    address_y = models.IntegerField(default=-1, blank=True)

    def __str__(self):
        return f'({self.address_x}, {self.address_y})'

class Product(models.Model):
    # product id using default serial int as primary key
    description = models.CharField(max_length=200, null = False, blank = False)
    category = models.ForeignKey(Category, related_name='products', on_delete=models.SET_NULL, null = True)
    price = models.FloatField(default = 9.99, null = False, blank = False)

    def __str__(self):
        return f'<{self.description}, {self.price}>'

class Package(models.Model):
    customer = models.ForeignKey(User, related_name='packages', on_delete=models.CASCADE)
    warehouse = models.ForeignKey(Warehouse, related_name='packages', on_delete=models.SET_NULL,null = True)
    address_x = models.IntegerField(default=1)
    address_y = models.IntegerField(default=1)
    pack_time = models.DateTimeField(default=timezone.now)
    status = models.CharField(max_length=50,default = "processing")
    ups_username = models.CharField(max_length=16,default = "", blank = True)
    track_num = models.IntegerField(default = -1)

    def __str__(self):
        return f"{self.customer}'s package at {self.warehouse}, {self.status}"

class Order(models.Model):
    customer = models.ForeignKey(User, related_name='orders', on_delete=models.CASCADE)
    product = models.ForeignKey(Product, related_name='orders', on_delete=models.SET_NULL, null = True)
    product_num = models.IntegerField(default = 1)
    package = models.ForeignKey(Package, related_name='orders', on_delete=models.SET_NULL, null = True)
    generate_time = models.DateTimeField(default=timezone.now)

    def __str__(self):
        return f'{self.product_num} {self.product} in {self.package}'
