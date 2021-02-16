from django.shortcuts import render, redirect
from django.contrib.auth.forms import UserCreationForm
from django.contrib import messages
from django.contrib.auth.decorators import login_required
from .forms import UserRegisterForm, UserUpdateForm, ProfileUpdateForm, VehicleUpdateForm, RideRequestForm, RideSearchForm
from django.core.mail import send_mail

from .models import Ride, Driver
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
        #v_form = VehicleUpdateForm(instance=request.user.vehicle)
        if u_form.is_valid() and p_form.is_valid() and v_form.is_valid():
            u_form.save()
            p_form.save()
            v_form.save()
            messages.success(request, f'Your account has been updated')
            return redirect('user_ride')
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

@login_required
def user_ride(request):
    ride_own = list(Ride.objects.filter(owner=request.user))
    ride_sharer = list(Ride.objects.filter(sharer = request.user))
    context = {
        'ride_own': ride_own, 
        'ride_share' : ride_sharer
    }
    return render(request, 'users/user_ride_request.html', context)


@login_required
def SharerJoin(request, ride_id):
    ride = Ride.objects.filter(pk=ride_id).first()
    ride.sharer.add(request.user)
    ride.save()
    return redirect('user_ride')


@login_required
def driver_search(request):
    context = {}
    if(request.user.profile.is_driver == '1'):
        rides = list(Ride.objects.filter(passenger_number__lte=request.user.vehicle.capacity, vehicle_type=request.user.vehicle.type, status='open'))
        context['rides'] = rides
        return render(request, 'users/driver_ride_search.html', context)
    else:
        messages.warning(request, f'Please register as a driver')
        return redirect('profile')
    # if(request.user.profile.is_driver == 'Driver'):
    #     rides = list(Ride.objects.filter(passenger_number__lte=request.user.vehicle.capacity, vehicle_type=request.user.vehicle.type, status='open'))
    #     context['rides'] = rides
    # else:
    #     rides = None
    #     context['rides'] = rides
    

@login_required
def user_ride_search(request):
    context = {}
    if(request.method == 'POST'):
        form = RideSearchForm(request.POST)
        #ride = None
        if form.is_valid():
            dest = form.cleaned_data['destination']
            early_time =  form.cleaned_data['earliest_time']
            late_time = form.cleaned_data['latest_time']
            passenger_number = form.cleaned_data['passenger_number']
            ride = Ride.objects.filter(arrival_time__range=(early_time,late_time), destination = dest, passenger_number__lt=passenger_number, status = 'open')
            #ride = Ride.objects.all()
            context['rides']=ride
            print(ride)
            return render(request, 'users/user_ride_result.html', {'rides':ride})
    else :
        form = RideSearchForm()
        return render(request, 'users/user_ride_search.html', {'form':form})

@login_required
def RideRequestModify(request, ride_id):
    if request.method == 'POST':
        ride = Ride.objects.filter(pk=ride_id).first()
        r_form = RideRequestForm(request.POST, instance=ride)
        if r_form.is_valid():
            r_form.save()
            messages.success(request, f'Your account has been updated')
            return redirect('user_ride')
    else:
        ride = Ride.objects.filter(pk=ride_id).first()
        #print(ride.)
        r_form = RideRequestForm(instance=ride)

    context = {
        'form' : r_form
    }
    return render(request, 'users/ride_request.html', context)

@login_required
def DriverClaim(request, ride_id):
    ride = Ride.objects.filter(pk=ride_id).first()
    ride.status = 'confirmed'
    print(ride.status)
    drive = Driver()
    drive.driver = request.user
    drive.ride = ride
    drive.save()
    ride.save()
    print(drive)
    email_list = []
    user = ride.owner
    sharer = ride.sharer.all()
    email_list.append(user.email)
    for s in sharer:
        email_list.append(s.email)
    
    #Email content:
    subject = 'Your Ride is Confirmed.'
    message = 'Here is the information of the ride\n'
    message += 'Destination: ' + ride.destination + '\n'
    message += 'Arrival time: ' + str(ride.arrival_time) + '\n'
    message += 'Driver: ' + request.user.username + '\n'
    message += 'Vehicle: ' + request.user.vehicle.type + '\n'

    #from_email = settings.EMAIL_HOST_USER
    print(message)
    print(email_list)
    send_mail(subject, message, '2500578242@qq.com', email_list, fail_silently=False)
    return redirect('driver_task')

@login_required
def DriverTask(request):
    drivers = list(Driver.objects.filter(driver=request.user).all())
    return render(request, 'users/driver_task.html', {'drivers':drivers})

@login_required
def DriverComplete(request, ride_id):
    ride = Ride.objects.filter(pk=ride_id).first()
    print(ride.status)
    ride.status='completed'
    print(ride.status)
    print(ride)
    ride.save()
    return redirect('driver_task') 


