from django.shortcuts import render
from .funcs import notify_backend
from .models import *
# Create your views here.

def checkout(request):
    result = notify_backend(1)
    return render(request,'amazonWeb/checkout.html',{'packageID':1, 'result':result})

def home(request):
    return render(request,'amazonWeb/base.html',{})


class htmlProduct:
    def __init__(self, description, category, price):
        self.description = description
        self.category = category
        self.price = price

def products(request):
    context = {}
    all_products = Product.objects.all()
    htmlProducts = []
    for product in all_products:
        cat = Category.objects.filter(id = product.category_id).first().category
        htmlProducts.append(htmlProduct(product.description,cat,product.price))
    context["products"]=htmlProducts
    return render(request,'amazonWeb/products.html',context)