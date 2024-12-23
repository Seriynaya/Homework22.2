from django.urls import path
from catalog import views

urlpatterns = [
    path('home/', views.home_render, name='home'),
    path('contacts/', views.contacts_render, name='contacts')
]