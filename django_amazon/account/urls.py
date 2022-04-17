from django.urls import path
from . import views
from django.contrib.auth import views as auth_views


urlpatterns = [
    path('register/',views.register, name = 'account-register'),
    path('login/',auth_views.LoginView.as_view(template_name='account/login.html'), name='account-login'),
    path('logout/',auth_views.LogoutView.as_view(template_name='account/logout.html'), name='account-logout'),
    path('profile/',views.profile, name = 'account-profile'),
    path('edit_profile/',views.edit_profile, name='account-profile-edit'),
    path('edit_optional/',views.edit_optional, name='account-profile-optional-edit'),
    path('add_address/',views.add_address, name='account-profile-add-address'),
    path('edit_address/',views.edit_address, name='account-profile-edit-address'),
]