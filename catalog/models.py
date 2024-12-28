from django.db import models

class Product(models.Model):
    name = models.CharField(max_length=150, verbose_name='наименование', help_text='Введите наименование')
    description = models.TextField(max_length=250, verbose_name='описание', help_text='Введите описание')
    image = models.ImageField(upload_to='images', blank=True, null=True, verbose_name='Изображение',
                              help_text='Загрузите изображение')
    category = models.ForeignKey("Category", on_delete=models.CASCADE, verbose_name='категория',
                                 help_text='Введите категорию', blank=True, null=True)
    price = models.CharField(max_length=150, verbose_name='Цена', help_text='Введите цену', blank=True, null=True)
    created_at = models.DateField(verbose_name='дата создания', help_text='Введите дату создания', blank=True,
                                  null=True)
    updated_at = models.DateTimeField(auto_now=True, verbose_name='дата последнего изменения', blank=True, null=True)

    def __str__(self):
        return f'{self.name}'

    class Meta:
        verbose_name = 'продукт'
        verbose_name_plural = 'продукты'


class Category(models.Model):
    name = models.CharField(max_length=150, verbose_name='название', help_text='Введите название', blank=True,
                            null=True)
    description = models.TextField(max_length=250, verbose_name='описание', help_text='Введите описание', blank=True,
                                   null=True)

    def __str__(self):
        return f'{self.name}'

    class Meta:
        verbose_name = 'категория'
        verbose_name_plural = 'категории'