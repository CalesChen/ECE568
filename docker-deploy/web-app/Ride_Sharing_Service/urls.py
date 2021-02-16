"""Ride_Sharing_Service URL Configuration

The `urlpatterns` list routes URLs to views. For more information please see:
    https://docs.djangoproject.com/en/3.1/topics/http/urls/
Examples:
Function views
    1. Add an import:  from my_app import views
    2. Add a URL to urlpatterns:  path('', views.home, name='home')
Class-based views
    1. Add an import:  from other_app.views import Home
    2. Add a URL to urlpatterns:  path('', Home.as_view(), name='home')
Including another URLconf
    1. Import the include() function: from django.urls import include, path
    2. Add a URL to urlpatterns:  path('blog/', include('blog.urls'))
"""
from django.contrib import admin
from django.contrib.auth import views as auth_views
from django.urls import include, path
from django.conf import settings
from django.conf.urls.static import static
from user import views as user_views

urlpatterns = [
    path('', include('polls.urls')),
    path('admin/', admin.site.urls),
    path('register/', user_views.register, name='register'),
    path('ride_request/', user_views.ride_request, name='ride_request'),
    path('user_ride/', user_views.user_ride, name='user_ride'),
    path('driver_search/', user_views.driver_search, name='driver_search'),
    path('user_ride_search/', user_views.user_ride_search, name = 'user_ride_search'),
    path('sharer_join/<int:ride_id>/', user_views.SharerJoin, name = 'sharer_join'),
    path('ride_request_modify/<int:ride_id>/', user_views.RideRequestModify, name = 'ride_request_modify'),
    path('driver_claim/<int:ride_id>/', user_views.DriverClaim, name = 'driver_claim'),
    path('driver_complete/<int:ride_id>/', user_views.DriverComplete, name = 'driver_complete'),
    path('driver_task/', user_views.DriverTask, name = 'driver_task'),
    #path('register/', include('user.urls')),
    path('profile/', user_views.profile, name='profile'),
    path('login/', auth_views.LoginView.as_view(template_name = 'users/login.html'), name='login'),
    path('logout/', auth_views.LogoutView.as_view(template_name = 'users/logout.html'), name='logout'),
]

if settings.DEBUG:
    urlpatterns += static(settings.MEDIA_URL, document_root=settings.MEDIA_ROOT)