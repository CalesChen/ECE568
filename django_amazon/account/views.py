from django.shortcuts import render, redirect
from django.http import HttpResponse
from django.contrib import messages
from django.contrib.auth.decorators import login_required
from .models import *
from .forms import *
# Create your views here.


def register(request):
    if request.method == 'POST':
        form = UserRegistrationForm(request.POST)
        if form.is_valid():
            user = form.save()
            useraccount = UserAccount(user=user)
            useraccount.save()
            messages.success(request, f'Your account has been created! You are now able to login!')
            return redirect('account-login')
        else:
            form = UserRegistrationForm()
            return render(request, 'account/register.html',{'form':form})
    else:
        form = UserRegistrationForm()
        return render(request, 'account/register.html',{'form':form})


@login_required
def profile(request):
    user = request.user
    context = {}
    context["username"] = user.username
    context["email"] = user.email
    userAccount = user.useraccount
    context["useraccount"] = userAccount
    not_initialized = userAccount.phone==0 and userAccount.cardInfo==0 and userAccount.ups_account_name==""
    context["useraccount_initialized"] = None if not_initialized else True
    context["addresses"] = userAccount.addresses.all() 
    return render(request, 'account/profile.html',context)

@login_required
def edit_profile(request):
    curr_user = request.user
    if request.method =='POST':
        form = Form(request.POST)
        if form.is_valid():
            curr_user.email = form.cleaned_data['email']
            curr_user.first_name = form.cleaned_data['first_name']
            curr_user.last_name = form.cleaned_data['last_name']
            curr_user.save()
            messages.success(request, f'Your profile has been updated!')
            return redirect('account-profile')
        else:
            form = EditProfileForm()
            return render(request, 'account/edit_profile.html',{'form':form})
    else:
        form = EditProfileForm()
        return render(request, 'account/edit_profile.html',{'form':form})
    

@login_required
def edit_optional(request):
    curr_user = request.user
    if request.method =='POST':
        form = EditOptionalForm(request.POST)
        if form.is_valid():
            curr_user.useraccount.phone = form.cleaned_data['phone']
            curr_user.useraccount.cardInfo = form.cleaned_data['cardInfo']
            curr_user.useraccount.ups_account_name = form.cleaned_data['ups_account_name']
            curr_user.useraccount.save()
            messages.success(request, f'Your optional Info has been updated!')
            return redirect('account-profile')
        else:
            form = EditOptionalForm()
            return render(request, 'account/edit_optional.html',{'form':form})
    else:
        form = EditOptionalForm()
        return render(request, 'account/edit_optional.html',{'form':form})
    

@login_required
def edit_address(request):
    return render(request, 'account/edit_address.html')

@login_required
def add_address(request):
    curr_user_account = request.user.useraccount
    if request.method =='POST':
        form = AddAddressForm(request.POST)
        if form.is_valid():
            name = form.cleaned_data['addressName']
            tag = form.cleaned_data['tag']
            if(tag=='my own tag'):
                tag = form.cleaned_data['myTag']
            address_x = form.cleaned_data['address_x']
            address_y = form.cleaned_data['address_y']
            address = Address(owner=curr_user_account,addressName=name,tag=tag,address_x=address_x,address_y=address_y)
            address.save()
            messages.success(request, f'Your optional Info has been updated!')
            return redirect('account-profile')
        else:
            form = AddAddressForm()
            return render(request, 'account/add_address.html',{'form':form})
    else:
        form = AddAddressForm()
        return render(request, 'account/add_address.html',{'form':form})