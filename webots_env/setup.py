from setuptools import setup, find_packages

package_name = 'webots_env'

setup(
    name=package_name,
    version='0.0.1',
    packages=find_packages(),
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='devan',
    maintainer_email='devan@example.com',
    description='Webots simple differential drive robot',
    license='Apache License 2.0',
)