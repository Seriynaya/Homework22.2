from django.urls import path
from catalog import views
from catalog.apps import CatalogConfig
app_name = CatalogConfig.name
urlpatterns = [
    path('home/', views.home_render, name='home'),
    path('contacts/', views.contacts_render, name='contacts')
]