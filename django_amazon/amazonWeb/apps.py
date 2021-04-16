from django.apps import AppConfig
from django.db.models.signals import post_migrate
from django.core.exceptions import ObjectDoesNotExist

def create_user(name,uEmail,pw):
    from django.contrib.auth.models import User 
    try:
        User.objects.get(username=name)
    except ObjectDoesNotExist:
        user = User.objects.create(
            username=name,
            email=uEmail,
            is_superuser=False
        )
        user.set_password(pw)
        user.save()

def create_category(names):
    from amazonWeb.models import Category
    if Category.objects.all().count()==0:
        for i in range(len(names)):
            Category.objects.create(category=name)

def create_product(descriptions,categories,prices):
    from amazonWeb.models import Product,Category
    assert len(descriptions)==len(prices) and len(descriptions)==len(categories)
    length = len(descriptions)
    if Product.objects.all().count()==0:
        for i in range(length):
            cat = Category.objects.get(category=categories[i])
            Product.objects.create(description=descriptions[i],
                category = cat,
                price = prices[i]
            )

def create_warehouse(xs,ys):
    from amazonWeb.models import Warehouse
    assert len(xs)==len(ys)
    if Warehouse.objects.all().count()==0:
        for i in range(len(xs)):
            Warehouse.objects.create(address_x=xs[i],address_y=ys[i])

def init_database(sender, **kwargs):
    create_user("XueyangLiu","xl350@duke.edu","xueyangliu321")
    create_user("KeChen","kc426@duke.edu","kechen321")
    create_category(["electronics","foods","books"])
    create_product(["macbookpro","biscuit","The Da Vinci Code"],["electronics","foods","books"],[2499,1.99,14.99])
    create_warehouse([1,2,3,4,5,6,7,8,9,10],[1,2,3,4,5,6,7,8,9,10])

class AmazonwebConfig(AppConfig):
    name = 'amazonWeb'
    
    def ready(self):
        post_migrate.connect(init_database, sender=self)