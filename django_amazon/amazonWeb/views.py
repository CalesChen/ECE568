from django.shortcuts import render
from .funcs import notify_backend
# Create your views here.

def checkout(request):
    result = notify_backend(1)
    return render(request,'amazonWeb/checkout.html',{'packageID':1, 'result':result})

def home(request):
    return render(request,'amazonWeb/base.html',{})