from django.shortcuts import render
from .funcs import *
from .models import *
from django.contrib.auth.decorators import login_required
# Create your views here.

@login_required
def checkout(request, product_id):

    address_x = 1
    address_y = 1
    whID = get_closest_wh(address_x, address_y)
    user = User.objects.all().first()
    wh = Warehouse.objects.filter(id=whID).first()
    product = Product.objects.filter(id = product_id).first()
    package = Package(customer = user,warehouse = wh,
        address_x = address_x, address_y = address_y)
    package.save()
    Order.objects.create(customer = user,product = product,
        package = package)
    result = notify_backend(package.id)
    return render(request,'amazonWeb/checkout.html',{'packageID':package.id, 'result':result})



class htmlProduct:
    def __init__(self, ID, description, category, price):
        self.id = ID
        self.description = description
        self.category = category
        self.price = price

def products(request):
    context = {}
    context["category"] = "all"
    all_products = Product.objects.all()
    htmlProducts = []
    for product in all_products:
        cat = Category.objects.filter(id = product.category_id).first().category
        htmlProducts.append(htmlProduct(product.id,product.description,cat,product.price))
    context["products"]=htmlProducts
    context["categories"] = Category.objects.all()
    return render(request,'amazonWeb/products.html',context)

def productsByCategory(request, category_id):
    context = {}
    if(category_id==-1):
        context["category"] = "all"
        all_products = Product.objects.all()
    else:
        cat = Category.objects.filter(id=category_id).first()
        context["category"] = cat.category
        all_products = cat.products.all()
    htmlProducts = []
    for product in all_products:
        cat = Category.objects.filter(id = product.category_id).first().category
        htmlProducts.append(htmlProduct(product.id,product.description,cat,product.price))
    context["products"]=htmlProducts
    context["categories"] = Category.objects.all()
    return render(request,'amazonWeb/products.html',context)