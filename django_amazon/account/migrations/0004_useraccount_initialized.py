# Generated by Django 3.1.5 on 2021-04-23 10:02

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('account', '0003_auto_20210422_1338'),
    ]

    operations = [
        migrations.AddField(
            model_name='useraccount',
            name='initialized',
            field=models.BooleanField(default=None),
        ),
    ]
