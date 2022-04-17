from django import forms 
from django.forms.fields import NumberInput


class PurchaseForm(forms.Form):
    productNum = forms.IntegerField(label='Number of products',required=True)
    address_x = forms.IntegerField(label='Address x',required=True)
    address_y = forms.IntegerField(label='Address y',required=True)