import { ReleaseObject } from "@hydrogen-web/interfaces";
import {
  Button,
  SegmentedControl,
  Select,
  Stack,
  Text,
  Title,
} from "@mantine/core";
import { Dropzone } from "@mantine/dropzone";
import { useForm } from "@mantine/form";
import { showNotification } from "@mantine/notifications";
import { useApi } from "hooks/useApi";
import { NextPage } from "next";
import { useEffect, useState } from "react";
import semver from "semver";

// &#8212;

const Page: NextPage = () => {
  const { fetch } = useApi();
  const [isLoading, setLoading] = useState(false);

  const form = useForm<{
    platform: string;
    version: string;
    selectedVersion: string;
    file?: File;
  }>({
    initialValues: {
      platform: "windows",
      version: "0.0.0",
      selectedVersion: "",
    },
    validate: {
      platform: (platform) => (!platform ? "Required" : null),
      selectedVersion: (selectedVersion) =>
        !selectedVersion ? "Required" : null,
      file: (file: any) => (!file ? "Required" : null),
    },
  });

  useEffect(() => {
    if (!form.values.platform) return;

    fetch<ReleaseObject>({
      method: "GET",
      url: `/releases/${form.values.platform}`,
    })
      .then((response) => {
        form.setFieldValue("version", response.data.version);
        form.setFieldValue("selectedVersion", "");
      })
      .catch(() => {
        form.setFieldValue("version", "0.0.0");
        form.setFieldValue("selectedVersion", "");
      });
  }, [form.values.platform]);

  return (
    <Stack
      p="xl"
      align="center"
      justify="center"
      sx={{
        flexGrow: 1,
      }}
    >
      <Stack
        sx={{
          minWidth: "80vw",
        }}
      >
        <Title size={30} weight={500}>
          Update Wizard
        </Title>
        <form
          onSubmit={form.onSubmit(async (data) => {
            setLoading(true);

            const formData = new FormData();
            formData.append("file", data.file!);

            try {
              await fetch({
                method: "POST",
                url: `/releases/${data.platform}/${data.selectedVersion}`,
                data: formData,
              });

              showNotification({
                title: "Released successfully",
                message: `Release added to "${data.platform}" bucket`,
                color: "green",
              });
            } catch (error: any) {
              showNotification({
                title: "Something went wrong",
                message: error.data.error.message,
                color: "red",
              });
            }

            form.reset();
            setLoading(false);
          })}
        >
          <Stack>
            <SegmentedControl
              disabled={isLoading}
              data={[
                { label: "Windows", value: "windows" },
                { label: "Android", value: "android" },
                { label: "macOS", value: "macos" },
                { label: "iOS", value: "ios" },
              ]}
              {...form.getInputProps("platform")}
            />

            <Select
              disabled={isLoading}
              placeholder={`Current version: ${form.values.version}`}
              data={[
                {
                  label: `major release - ${semver.inc(
                    form.values.version,
                    "major"
                  )}`,
                  value: semver.inc(form.values.version, "major")!,
                },
                {
                  label: `minor release - ${semver.inc(
                    form.values.version,
                    "minor"
                  )}`,
                  value: semver.inc(form.values.version, "minor")!,
                },
                {
                  label: `patch release - ${semver.inc(
                    form.values.version,
                    "patch"
                  )}`,
                  value: semver.inc(form.values.version, "patch")!,
                },
              ]}
              {...form.getInputProps("selectedVersion")}
            />

            <Stack spacing={3}>
              <Dropzone
                disabled={isLoading}
                maxFiles={1}
                onDrop={(files) => {
                  form.setFieldValue("file", files[0]);
                }}
                sx={{
                  borderColor: !!form.errors.file ? "#e03131" : undefined,
                }}
              >
                <Text
                  size="md"
                  color={!!form.errors.file ? "#e03131" : "dimmed"}
                >
                  {form.values.file
                    ? form.values.file.name
                    : "Drag or click to select file"}
                </Text>
              </Dropzone>
              {form.errors.file ? (
                <Text size="xs" color="#e03131">
                  {form.errors.file}
                </Text>
              ) : null}
            </Stack>

            <Button
              loading={isLoading}
              type="submit"
              radius="sm"
              size="md"
              color="dark"
            >
              Upload Release
            </Button>
          </Stack>
        </form>
      </Stack>
    </Stack>
  );
};

export default Page;
