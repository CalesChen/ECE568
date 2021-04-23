from django.urls import path
from . import views

urlpatterns = [
    path('before_checkout/<int:product_id>/',views.before_checkout, name = 'amazon-before-checkout'),
    path('category/<int:category_id>/',views.productsByCategory, name='amazon-categoryView'),
    path('',views.products, name = 'amazon-products'),
    path('orders/',views.allOrdersAndPackages, name = 'amazon-orders'),
]