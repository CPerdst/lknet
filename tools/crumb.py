import requests

url = 'https://l1akr.jenkins.com/crumbIssuer/api/json'

username='l1Akr'

api_token='11087356f1a6b18ffb9aa4245748d7943b'

response = requests.get(url=url, auth=(username, api_token), verify=False).json()

crumb = response['crumb']

header_filed = response['crumbRequestField']

url = 'https://l1akr.jenkins.com/job/lknet-boost-env/buildWithParameters?token=asd-123'

headers = {
    header_filed: crumb
}

response = requests.post(url=url, auth=(username, api_token), headers=headers, verify=False)

print(response.text)