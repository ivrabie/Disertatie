from django.contrib.auth.models import User, Group
from rest_framework import viewsets
from quickstart.serializers import UserSerializer, GroupSerializer
import os
import mimetypes
from django.http import StreamingHttpResponse
from wsgiref.util import FileWrapper
from rest_framework.decorators import action
class UserViewSet(viewsets.ModelViewSet):
    """
    API endpoint that allows users to be viewed or edited.
    """
    queryset = User.objects.all().order_by('-date_joined')
    serializer_class = UserSerializer




class GroupViewSet(viewsets.ModelViewSet):
    """
    API endpoint that allows groups to be viewed or edited.
    """
    queryset = Group.objects.all()
    serializer_class = GroupSerializer


class DownloadViewSet(viewsets.ViewSet):
    queryset = User.objects.all().order_by('-date_joined')

    # @action(detail=True, methods=['get'])
    def list(self,request):
        the_file = 'd:\\Disertatie\\Disertatie\\gateway\\build\\gateway.bin'
        filename = os.path.basename(the_file)
        chunk_size = 2048
        response = StreamingHttpResponse(FileWrapper(open(the_file, 'rb'), chunk_size),
                                content_type=mimetypes.guess_type(the_file)[0])
        response['Content-Length'] = os.path.getsize(the_file)    
        response['Content-Disposition'] = "attachment; filename=%s" % filename
        return response
