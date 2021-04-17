from django.urls import path
from . import views

urlpatterns = [
    path('checkout/',views.checkout, name = 'amazon-checkout'),
    path('',views.home, name = 'amazon-home'),
    path('products/',views.products, name = 'amazon-products'),
]