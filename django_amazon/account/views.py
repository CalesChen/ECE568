from django.shortcuts import render, redirect
from django.http import HttpResponse
from django.contrib import messages
from django.contrib.auth.decorators import login_required
from .models import *
from .forms import *
# Create your views here.

def home(request):
    return HttpResponse('<h1> Account Home</h1>')

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
    return render(request, 'account/profile.html')