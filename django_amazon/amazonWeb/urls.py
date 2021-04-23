from django.urls import path
from . import views

urlpatterns = [
    path('checkout/<int:product_id>/',views.checkout, name = 'amazon-checkout'),
    path('category/<int:category_id>/',views.productsByCategory, name='amazon-categoryView'),
    path('',views.products, name = 'amazon-products'),
]