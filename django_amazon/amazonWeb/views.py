from django.shortcuts import render,redirect
from django.http import HttpResponse
from .funcs import *
from .models import *
from .forms import *
from django.core.mail import send_mail
from django.contrib import messages
from django.contrib.auth.decorators import login_required
# Create your views here.


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


class OrderAndPack:
    def __init__(self, order, pack, product):
        self.order = order
        self.package = pack
        self.product = product

@login_required
def allOrdersAndPackages(request):
    context = {}
    user = request.user
    orders = Order.objects.filter(customer=user).order_by('-generate_time')
    combines = []
    for order in orders:
        pack = order.package
        product = order.product
        combines.append(OrderAndPack(order,pack,product))
    context["OrdersAndPacks"] = combines
    return render(request,'amazonWeb/all_orders.html',context)

@login_required
def before_checkout(request, product_id):
    user = request.user
    if user.useraccount.ups_userid==-2:
        messages.warning(request, f"The ups is verifying your information!")
        return redirect('account-profile')
    if user.useraccount.ups_userid==-1:
        messages.warning(request, f"You have to fill in the optional info to purchase!")
        return redirect('account-profile')
    if request.method =='POST':
        form = PurchaseForm(request.POST)
        if form.is_valid():
            address_x = form.cleaned_data["address_x"]
            address_y = form.cleaned_data["address_y"]
            product_num = form.cleaned_data["productNum"]
            whID = get_closest_wh(address_x, address_y)
            wh = Warehouse.objects.filter(id=whID).first()
            product = Product.objects.filter(id = product_id).first()
            package = Package(customer = user,warehouse = wh, address_x = address_x, 
                address_y = address_y, ups_username=user.useraccount.ups_username)
            package.save()
            order = Order(customer = user,product = product,package = package, product_num=product_num)
            order.save()
            result = notify_backend(package.id)
            if result:
                # subject = "Your order has been placed!"
                # content = "You have ordered "+ product_num + product.description+"\n"
                # content += "Delivering to "+wh+"\n"
                # content += "Best,\nMini Amazon Team\n"
                # from_email = settings.EMAIL_HOST_USER
                # email_list = [user.email]
                # send_mail(subject,content,from_email,email_list,fail_silently=False)
                messages.success(request,f'You have successfully put the order!')
                return redirect('amazon-products')
            else:
                package.delete()
                order.delete()
                messages.warning(request,f'Something failed! Please try again!')
                return redirect('amazon-products')
        else:
            form = PurchaseForm()
            return render(request,'amazonWeb/before_checkout.html',{'form':form,'product_id':product_id})
    else:
        form = PurchaseForm()
        return render(request,'amazonWeb/before_checkout.html',{'form':form,'product_id':product_id})