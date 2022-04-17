from django.contrib import admin
from amazonWeb.models import *
# Register your models here.
admin.site.register(Category)
admin.site.register(Warehouse)
admin.site.register(Product)
admin.site.register(Package)
admin.site.register(Order)