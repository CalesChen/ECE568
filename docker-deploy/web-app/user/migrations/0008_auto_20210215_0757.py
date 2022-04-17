# Generated by Django 3.1.6 on 2021-02-15 07:57

from django.conf import settings
from django.db import migrations, models
import django.db.models.deletion


class Migration(migrations.Migration):

    dependencies = [
        migrations.swappable_dependency(settings.AUTH_USER_MODEL),
        ('user', '0007_vehicle'),
    ]

    operations = [
        migrations.AlterField(
            model_name='vehicle',
            name='capacity',
            field=models.IntegerField(default=4),
        ),
        migrations.AlterField(
            model_name='vehicle',
            name='comment',
            field=models.CharField(default='000', max_length=100),
        ),
        migrations.AlterField(
            model_name='vehicle',
            name='license_number',
            field=models.CharField(default='000', max_length=50),
        ),
        migrations.CreateModel(
            name='Ride',
            fields=[
                ('id', models.AutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('destination', models.CharField(max_length=100)),
                ('arrival_time', models.DateTimeField()),
                ('passenger_num', models.IntegerField()),
                ('vehicle_type', models.CharField(blank=True, default='Car', max_length=50)),
                ('special_request', models.CharField(blank=True, max_length=100)),
                ('can_Shared', models.BooleanField(default=False)),
                ('status', models.CharField(choices=[('open', 'open'), ('confirmed', 'confirmed'), ('completed', 'completed')], default='open', max_length=20)),
                ('owner', models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, to=settings.AUTH_USER_MODEL)),
            ],
        ),
    ]