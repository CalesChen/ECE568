from django.shortcuts import render, redirect
from django.contrib.auth.forms import UserCreationForm
from django.contrib import messages
from django.contrib.auth.decorators import login_required
from .forms import UserRegisterForm, UserUpdateForm, ProfileUpdateForm, VehicleUpdateForm, RideRequestForm

from .models import Ride
from django.http import Http404, HttpResponseRedirect, HttpResponse

from django.shortcuts import get_object_or_404, render, redirect
from datetime import datetime
from django.utils import timezone

from django.contrib.auth.mixins import LoginRequiredMixin
from django.contrib.auth.decorators import permission_required, login_required
from django.urls import reverse

# Create your views here.
def register(request):
    if request.method == 'POST':
        form = UserRegisterForm(request.POST)
        if form.is_valid():
            form.save()
            username = form.cleaned_data.get('username')
            messages.success(request, f'Your account has been created! You are now able to log in')
            return redirect('login')
    else:
        form = UserRegisterForm()
    return render(request, 'users/register.html', {'form':form})

@login_required
def profile(request):

    if request.method == 'POST':
        u_form = UserUpdateForm(request.POST, instance=request.user)
        p_form = ProfileUpdateForm(request.POST, instance=request.user.profile)
        v_form = VehicleUpdateForm(request.POST, instance=request.user.vehicle)
        v_form = VehicleUpdateForm(instance=request.user.vehicle)
        if u_form.is_valid() and p_form.is_valid() and v_form.is_valid():
            u_form.save()
            p_form.save()
            v_form.save()
            messages.success(request, f'Your account has been updated')
            return redirect('profile')
    else:
        u_form = UserUpdateForm(instance=request.user)
        p_form = ProfileUpdateForm(instance=request.user.profile)
        v_form = VehicleUpdateForm(instance=request.user.vehicle)

    context = {
        'u_form' : u_form,
        'p_form' : p_form,
        'v_form' : v_form
    }
    return render(request, 'users/profile.html', context)

@login_required()
def ride_request(request):
    ride = Ride()
    if request.method == 'POST':
        form = RideRequestForm(request.POST)
        if form.is_valid():
            ride.destination = form.cleaned_data['destination']
            ride.arrival_time = form.cleaned_data['arrival_time']
            currenttime = timezone.now()
            if ride.arrival_time < currenttime:
                messages.warning(request, f'Your time is invalid')
                return render(request,'users/ride_request.html', {'form':form})
                
            ride.owner = request.user
            ride.passenger_number = form.cleaned_data['passenger_number']
            ride.special_request = form.cleaned_data['special_request']
            ride.vehicle_type = form.cleaned_data['vehicle_type']
            share = request.POST.get("can_Shared")
            if share == "no":
                ride.can_Shared = False
            else:
                ride.can_shared = True
            ride.status = 'open'
            ride.save()
            return HttpResponseRedirect(reverse('profile'))
    else:
        form = RideRequestForm()
    return render(request, 'users/ride_request.html', {'form':form})