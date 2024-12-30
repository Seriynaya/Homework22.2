from django.shortcuts import render


def home_render(request):
    return render(request, 'catalog/home.html')


def contacts_render(request):
    return render(request, 'catalog/contacts.html')