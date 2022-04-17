from django.urls import path

from . import views

urlpatterns = [
    path('', views.register, name = 'user-register'),
    #path('about/', views.about, name = 'polls-about'),
]