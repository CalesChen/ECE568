# Generated by Django 3.1.6 on 2021-02-15 05:05

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('user', '0005_auto_20210215_0126'),
    ]

    operations = [
        migrations.AlterField(
            model_name='profile',
            name='is_driver',
            field=models.CharField(default='User', max_length=10),
        ),
    ]